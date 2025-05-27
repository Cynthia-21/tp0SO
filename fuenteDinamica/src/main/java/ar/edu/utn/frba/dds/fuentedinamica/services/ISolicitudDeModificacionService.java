package ar.edu.utn.frba.dds.fuentedinamica.services;

import ar.edu.utn.frba.dds.fuentedinamica.models.domain.Administrador;
import ar.edu.utn.frba.dds.fuentedinamica.models.domain.Contribuyente;
import ar.edu.utn.frba.dds.fuentedinamica.models.domain.Hecho;
import ar.edu.utn.frba.dds.fuentedinamica.models.domain.solicitudes.PosibleEstado;
import ar.edu.utn.frba.dds.fuentedinamica.models.domain.solicitudes.SolicitudDeModificacion;
import ar.edu.utn.frba.dds.fuentedinamica.models.dtos.output.SolicitudDeModificacionOutputDTO;

import java.util.List;

public interface ISolicitudDeModificacionService {
  SolicitudDeModificacionOutputDTO crearSolicitud(Contribuyente solicitante, Hecho hechoARevisar, Hecho hechoModificado);
  List<SolicitudDeModificacionOutputDTO> obtenerSolicitudes();
  List<SolicitudDeModificacionOutputDTO> filtrarPorEstado(PosibleEstado estado);
  SolicitudDeModificacionOutputDTO buscarPorId(Long id);
  void aceptarSolicitud(Long id, Administrador admin);
  void aceptarConSugerencia(Long id, Administrador admin, String comentario);
  void rechazarSolicitud(Long id, Administrador admin);

}
