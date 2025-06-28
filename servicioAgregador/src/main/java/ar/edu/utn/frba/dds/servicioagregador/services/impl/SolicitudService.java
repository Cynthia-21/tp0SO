package ar.edu.utn.frba.dds.servicioagregador.services.impl;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Administrador;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.Contribuyente;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.Archivo;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.Fuente;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.FuenteDinamica;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.FuenteEstatica;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.FuenteProxy;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.origen.TipoFuente;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.solicitudes.EstadoSolicitud;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.solicitudes.MotivoSolicitud;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.solicitudes.SolicitudEliminacion;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.ContribuyenteDto;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.FuenteDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.input.EstadoSolicitudInputDto;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.input.SolicitudEliminacionInputDto;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.output.EstadoSolicitudOutputDto;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.output.SolicitudEliminacionOutputDto;
import ar.edu.utn.frba.dds.servicioagregador.models.repositories.IHechoRepository;
import ar.edu.utn.frba.dds.servicioagregador.models.repositories.ISolicitudRepository;
import ar.edu.utn.frba.dds.servicioagregador.services.ISolicitudService;
import ar.edu.utn.frba.dds.servicioagregador.services.spam.DetectorDeSpam;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
import java.util.List;
import java.util.stream.Collectors;

@Service
public class SolicitudService implements ISolicitudService {
  private final ISolicitudRepository solicitudRepository;
  private final IHechoRepository hechoRepository;
  private final DetectorDeSpam detectorDeSpam;

  public SolicitudService(ISolicitudRepository solicitudRepository, IHechoRepository hechoRepository, DetectorDeSpam detectorDeSpam) {
    this.solicitudRepository = solicitudRepository;
    this.hechoRepository = hechoRepository;
    this.detectorDeSpam = detectorDeSpam;
  }

  @Override
  public SolicitudEliminacionOutputDto crearSolicitud(SolicitudEliminacionInputDto solicitud) {
    SolicitudEliminacion solicitudEliminacion = new SolicitudEliminacion(
        Contribuyente.builder()
            .nombre(solicitud.getContribuyente().getNombre())
            .apellido(solicitud.getContribuyente().getApellido())
            .build(),
        this.hechoRepository.findById(solicitud.getHechoId()),
        new MotivoSolicitud(solicitud.getMotivo()));

    if (this.detectorDeSpam.esSpam(solicitud.getMotivo())) {
      solicitudEliminacion.rechazarPorSpam(LocalDateTime.now());
    }

    this.solicitudRepository.save(solicitudEliminacion);
    return this.solicitudToDto(solicitudEliminacion);
  }

  @Override
  public EstadoSolicitudOutputDto cambiarEstadoSolicitud(Long solicitudId, EstadoSolicitudInputDto estadoSolicitudDto) {
    SolicitudEliminacion solicitudEliminacion = this.solicitudRepository.findById(solicitudId);
    Hecho hechoEliminado = solicitudEliminacion.getHechoAEliminar();

    if (estadoSolicitudDto.getEstado().equals("CONFIRMADA")) {
      solicitudEliminacion.aceptarSolicitud(estadoSolicitudDto.getFechaDeRevision(), new Administrador(estadoSolicitudDto.getAdministrador()));
      this.mapTipoFuente(hechoEliminado.getTipoFuente()).eliminarHecho(hechoEliminado); // elimina en la fuente
    } else if (estadoSolicitudDto.getEstado().equals("RECHAZADA")) {
      solicitudEliminacion.rechazarSolicitud(estadoSolicitudDto.getFechaDeRevision(), new Administrador(estadoSolicitudDto.getAdministrador()));
    } else {
      throw new IllegalArgumentException("El estado no es valido");
    }

    this.hechoRepository.save(solicitudEliminacion.getHechoAEliminar());
    this.solicitudRepository.save(solicitudEliminacion);
    return this.estadoToDto(solicitudEliminacion.getUltimoEstadoSolicitud());
  }

  @Override
  public SolicitudEliminacionOutputDto conseguirSolicitud(Long id) {
    return this.solicitudToDto(this.solicitudRepository.findById(id));
  }

  @Override
  public List<EstadoSolicitudOutputDto> obtenerEstadosDeSolicitudId(Long id) {

    return this.solicitudRepository.findEstadosByIdSolicitud(id)
        .stream()
        .map(this::estadoToDto)
        .collect(Collectors.toList());
  }


  // -> TO DTOS
  private SolicitudEliminacionOutputDto solicitudToDto(SolicitudEliminacion solicitud) {
    SolicitudEliminacionOutputDto outputDto = new SolicitudEliminacionOutputDto();
    outputDto.setId(solicitud.getId());
    outputDto.setHechoId(solicitud.getHechoAEliminar().getId());
    outputDto.setContribuyente(new ContribuyenteDto().builder()
        .nombre(solicitud.getSolicitante().getNombre())
        .apellido(solicitud.getSolicitante().getApellido())
        .build());
    outputDto.setMotivo(solicitud.getMotivoSolicitud().getDescripcion());
    outputDto.setFechaAlta(solicitud.getFechaSolicitud());
    return outputDto;
  }

  private EstadoSolicitudOutputDto estadoToDto(EstadoSolicitud estadoSolicitud) {
    EstadoSolicitudOutputDto outputDto = new EstadoSolicitudOutputDto();
    outputDto.setAdministrador(estadoSolicitud.getAdministrador().getNombre());
    outputDto.setFechaDeRevision(estadoSolicitud.getFechaDeRevision());
    outputDto.setEstado(estadoSolicitud.getPosibleEstado().name());
    return outputDto;
  }


  // mappeos
  private Fuente mapTipoFuente(TipoFuente tipoFuente) {
    return switch (tipoFuente) {
      case ESTATICA -> new FuenteEstatica();
      case DINAMICA -> new FuenteDinamica();
      case PROXY    -> new FuenteProxy();
      default         -> throw new IllegalArgumentException("Tipo de fuente inválido");
    };
  }
}