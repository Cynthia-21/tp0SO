package ar.edu.utn.frba.dds.fuenteestatica.models.repositories;

import ar.edu.utn.frba.dds.fuenteestatica.models.domain.Hecho;
import java.util.List;

public interface IHechoRepository {
  public void save(Hecho unHecho);
  public List<Hecho> findAll();
  public List<Hecho> filtrarPorArchivo(String nombreArchivo);
}
