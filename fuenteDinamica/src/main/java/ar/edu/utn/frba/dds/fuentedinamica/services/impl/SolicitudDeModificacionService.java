package ar.edu.utn.frba.dds.fuentedinamica.services.impl;

import ar.edu.utn.frba.dds.fuentedinamica.models.domain.Administrador;
import ar.edu.utn.frba.dds.fuentedinamica.models.domain.Contribuyente;
import ar.edu.utn.frba.dds.fuentedinamica.models.domain.Hecho;
import ar.edu.utn.frba.dds.fuentedinamica.models.domain.exceptions.SolicitudNoEncontradaException;
import ar.edu.utn.frba.dds.fuentedinamica.models.domain.solicitudes.PosibleEstado;
import ar.edu.utn.frba.dds.fuentedinamica.models.domain.solicitudes.SolicitudDeModificacion;
import ar.edu.utn.frba.dds.fuentedinamica.models.dtos.output.HechoOutputDTO;
import ar.edu.utn.frba.dds.fuentedinamica.models.dtos.output.SolicitudDeModificacionOutputDTO;
import ar.edu.utn.frba.dds.fuentedinamica.models.repositories.impl.HechoRepository;
import ar.edu.utn.frba.dds.fuentedinamica.models.repositories.impl.SolicitudDeModificacionRepository;
import ar.edu.utn.frba.dds.fuentedinamica.services.ISolicitudDeModificacionService;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
import java.util.List;
import java.util.stream.Collectors;

@Service
public class SolicitudDeModificacionService implements ISolicitudDeModificacionService {

  private final SolicitudDeModificacionRepository solicitudRepository;
  private final HechoRepository hechoRepository;

  public SolicitudDeModificacionService(SolicitudDeModificacionRepository solicitudRepository, HechoRepository hechoRepository) {
    this.solicitudRepository = solicitudRepository;
    this.hechoRepository = hechoRepository;
  }

  @Override
  public SolicitudDeModificacionOutputDTO crearSolicitud(Contribuyente solicitante, Hecho hechoARevisar, Hecho hechoModificado) {
    SolicitudDeModificacion solicitud = new SolicitudDeModificacion(solicitante, hechoARevisar, hechoModificado);
    SolicitudDeModificacion nuevaSolicitud = solicitudRepository.save(solicitud);
    return solicitudDeModificacionOutputDTO(nuevaSolicitud);
  }

  @Override
  public List<SolicitudDeModificacionOutputDTO> obtenerSolicitudes(){
    return this.solicitudRepository
        .findAll()
        .stream()
        .map(this::solicitudDeModificacionOutputDTO)
        .toList();
  }

  @Override
  public SolicitudDeModificacionOutputDTO buscarPorId(Long id) {
    var solicitud = this.solicitudRepository.findById(id);
    return this.solicitudDeModificacionOutputDTO(solicitud);
  }

  @Override
  public List<SolicitudDeModificacionOutputDTO> filtrarPorEstado(PosibleEstado estado) {
    return solicitudRepository
        .findAll()
        .stream()
        .filter(solicitud -> solicitud.getEstadoRevisionActual() == estado)
        .map(this::solicitudDeModificacionOutputDTO)
        .collect(Collectors.toList());

  }

  @Override
  public void aceptarSolicitud(Long id, Administrador administrador) {
    SolicitudDeModificacion solicitud = solicitudRepository.findById(id);
    if (solicitud == null) {
      throw new SolicitudNoEncontradaException();
    }
    solicitud.aceptarRevision(LocalDateTime.now(), administrador);
    solicitudRepository.save(solicitud);
    hechoRepository.save(solicitud.getHechoModificado());
  }

  @Override
  public void aceptarConSugerencia(Long id, Administrador administrador, String comentario) {
    SolicitudDeModificacion solicitud = solicitudRepository.findById(id);
    if (solicitud == null) {
      throw new SolicitudNoEncontradaException();
    }
    solicitud.aceptarConSugerencia(LocalDateTime.now(), administrador, comentario);
    solicitudRepository.save(solicitud);
    hechoRepository.save(solicitud.getHechoModificado());
  }

  @Override
  public void rechazarSolicitud(Long id, Administrador administrador) {
    SolicitudDeModificacion solicitud = solicitudRepository.findById(id);
    if (solicitud == null) {
      throw new SolicitudNoEncontradaException();
    }
    solicitud.rechazarRevision(LocalDateTime.now(), administrador);
    solicitudRepository.save(solicitud);
  }

  private SolicitudDeModificacionOutputDTO solicitudDeModificacionOutputDTO(SolicitudDeModificacion solicitud) {
    SolicitudDeModificacionOutputDTO solicitudDeModificacionOutputDTO= new SolicitudDeModificacionOutputDTO();
    solicitud.setId(solicitud.getId());
    solicitud.setHechoARevisar(solicitud.getHechoARevisar());
    solicitud.setHechoModificado(solicitud.getHechoModificado());
    solicitud.setFechaDeAlta(solicitud.getFechaDeAlta());
    solicitud.setEstadosRevision(solicitud.getEstadosRevision());
    solicitud.setHistorialModificados(solicitud.getHistorialModificados());

    return solicitudDeModificacionOutputDTO;
  }
}
